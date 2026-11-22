# Fish Audio relay for StopWatch

Send a POST to the Worker endpoint with `Authorization: Bearer <DEVICE_TOKEN>`
and `Content-Type: application/json`. The body is `{"text":"こんにちは"}`.
The relay accepts up to 500 Unicode scalar values and streams MP3 as
`audio/mpeg`. It allows 10 authenticated requests per minute per isolate.

## Worker secrets

Set these three names as Wrangler secrets for the Worker.

- `FISH_AUDIO_API_KEY`
- `FISH_AUDIO_REFERENCE_ID`
- `DEVICE_TOKEN`

## StopWatch configuration

Create the gitignored `esp32/sdkconfig.local` with the deployed Worker URL
and the same device token. Replace both placeholders.

```ini
CONFIG_MUSE_FISH_TTS_URL="https://muse-fish-tts.example.workers.dev"
CONFIG_MUSE_FISH_TTS_TOKEN="<DEVICE_TOKEN>"
```

Use an HTTPS URL without a query string or fragment. Keep the configured
maximum ESP-IDF log level at INFO or below because the HTTP client logs request
headers at DEBUG. The Fish API key and reference ID belong on the Worker.

Run `esp32/tools/muse/board.sh build stopwatch` from the repository root.
The board script appends the local file to `SDKCONFIG_DEFAULTS` when present.
Use a fresh generated board config when adding or changing the local file,
since ESP-IDF can retain values from an existing build's `sdkconfig`.
Empty URL or token settings give captions-only replies.
