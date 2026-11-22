// A deployment accepts one device token, so one counter per isolate suffices.
let windowStart = 0;
let requests = 0;

function reply(message, status) {
  return new Response(message, { status });
}

export default {
  async fetch(request, env) {
    if (request.method !== "POST") return reply("Method not allowed", 405);
    if (!env.DEVICE_TOKEN ||
        request.headers.get("Authorization") !== `Bearer ${env.DEVICE_TOKEN}`) {
      return reply("Unauthorized", 401);
    }

    const now = Date.now();
    if (now - windowStart >= 60_000) {
      windowStart = now;
      requests = 0;
    }
    if (requests >= 10) return reply("Too many requests", 429);
    requests++;

    let text;
    try {
      ({ text } = await request.json());
    } catch {
      return reply("Invalid JSON", 400);
    }
    if (typeof text !== "string" || !text.trim()) return reply("Invalid text", 400);
    let characters = 0;
    for (const char of text) {
      const scalar = char.codePointAt(0);
      if (++characters > 500 || (scalar >= 0xd800 && scalar <= 0xdfff)) {
        return reply("Invalid text", 400);
      }
    }

    if (!env.FISH_AUDIO_API_KEY || !env.FISH_AUDIO_REFERENCE_ID) {
      console.log("missing_fish_secrets", Boolean(env.FISH_AUDIO_API_KEY), Boolean(env.FISH_AUDIO_REFERENCE_ID));
      return reply("Speech unavailable", 502);
    }
    const upstream = new AbortController();
    const timer = setTimeout(() => upstream.abort(), 15_000);
    try {
      const response = await fetch("https://api.fish.audio/v1/tts", {
        method: "POST",
        redirect: "follow",
        signal: upstream.signal,
        headers: {
          Authorization: `Bearer ${env.FISH_AUDIO_API_KEY}`,
          "Content-Type": "application/json",
          model: "s2.1-pro",
        },
        body: JSON.stringify({
          text,
          reference_id: env.FISH_AUDIO_REFERENCE_ID,
          format: "mp3",
          mp3_bitrate: 64,
          latency: "balanced",
          language: "ja",
        }),
      });
      if (!response.ok || !response.body) {
        console.log("fish_status", response.status);
        upstream.abort();
        return reply("Speech unavailable", 502);
      }

      // Check for an empty body before committing to the audio response.
      const reader = response.body.getReader();
      let first;
      do {
        first = await reader.read();
        if (first.done) {
          console.log("fish_empty_body");
          return reply("Speech unavailable", 502);
        }
      } while (!first.value.byteLength);

      const audio = new ReadableStream({
        start(controller) {
          controller.enqueue(first.value);
        },
        async pull(controller) {
          const { done, value } = await reader.read();
          if (done) controller.close();
          else controller.enqueue(value);
        },
        cancel(reason) {
          return reader.cancel(reason);
        },
      });
      return new Response(audio, {
        headers: { "Content-Type": "audio/mpeg", "Cache-Control": "no-store" },
      });
    } catch (err) {
      console.log("fish_exc", err && err.name, String((err && err.message) || err).slice(0, 160));
      return upstream.signal.aborted
        ? reply("Speech timed out", 504)
        : reply("Speech unavailable", 502);
    } finally {
      clearTimeout(timer);
    }
  },
};
