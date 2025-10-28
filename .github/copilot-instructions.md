## Quick orientation — OpenAI Metatag Assistant

This repository is a Backdrop CMS contrib module that injects a "Generate Metatags with OpenAI" button into node edit forms and can auto-generate missing metatags on save. The guidance below is intentionally focused and code-centric so an AI coding agent can be immediately productive.

Key files
- `openai_metatag.module` — main logic: form alters, after_build handler, AJAX callback, generation, and `hook_node_presave()`.
- `openai_metatag.info` — module metadata and declared dependencies (`openai`, `metatag`, `key`).
- `config/install/openai_metatag.settings.json` — default settings (enabled bundles, auto-generate, default model/tokens) if present; defaults also exist in code.
- `README.md` — high-level usage and installation notes. Use it for user-facing behavior.

Big picture / architecture
- Platform: Backdrop CMS 1.x module. This is not a standalone app — it hooks into Backdrop hooks and other modules.
- Integrations: relies on the `openai` module for API calls (reads API key via `key_get_key_value()`), `metatag` for UI/field definitions, and `key` for secret storage.
- Flow: `hook_form_alter()` -> attach the `openai_metatag_after_build` after-build handler -> inject a submit button with `#ajax` -> `openai_metatag_generate_submit()` collects context and calls `openai_metatag_generate_suggestions()` -> parsed suggestions populate the metatag form values for AJAX rebuild. Separately, `hook_node_presave()` runs auto-generation when enabled.

Project-specific patterns and conventions (explicit, discoverable)
- Two-stage form injection: the module uses both `hook_form_alter()` and an `#after_build` handler to reliably place the button after Metatag has built its UI. Search for `openai_metatag_after_build` and `#after_build` in `openai_metatag.module` for examples.
- Language container handling: metatags are namespaced by language (e.g. `metatags[und]`); the code computes a wrapper id `openai-metatag-wrapper-<lang>` and returns that container from the AJAX callback. See `openai_metatag_generate_ajax()`.
- Finding and setting values: the module performs a recursive search for `['value']` element arrays in nested groupings (see `openai_metatag_find_value_element_ref()` and `openai_metatag_collect_available_keys_recursive()`); use these helpers if you need to programmatically set metatag values on form rebuilds.
- Text extraction: the module traverses form values and entity fields via `openai_metatag_collect_text_from_values()` and `openai_metatag_collect_text_from_entity()` to gather context for the model — replicate this approach when adding more contextual signals.
- Config patterns: settings are read with `config_get('openai_metatag.settings', ...)` and normalized in `openai_metatag_settings_form_submit()`; the module falls back to `openai.settings` for global model options.
- Permissions: the runtime gate is `generate metatags with openai` (see `openai_metatag_permission()`). The module also checks typical Metatag permissions `edit meta tags` / `administer meta tags`.

Debugging and developer workflows (discoverable)
- To reproduce/inspect user-facing behavior: enable `openai`, `metatag`, `key`, then enable this module. The admin settings live at `admin/config/openai/metatag` (see `openai_metatag_settings_form()`).
- If the Generate button does not appear: check that Metatag fields exist on the content type and that the current user has the required permissions. The module logs diagnostic messages via Backdrop `watchdog()` — inspect recent logs in Backdrop or your PHP error log.
- OpenAI/API issues: the module obtains an API key name from `config('openai.settings')->get('api_key')` then `key_get_key_value()`; if generation fails, `watchdog()` records the exception message. Verify keys via the `key` module UI or the `openai` module settings.
- Runtime hints: the module intentionally avoids overwriting non-empty values. Auto-generation runs in `hook_node_presave()` only when `auto_generate_on_save` is enabled in config.

Testing / changes you might make
- Small, safe changes to experiment with behavior: change `ai_max_tokens` in `config/install/openai_metatag.settings.json` or via the admin UI, or temporarily add `watchdog()` calls for extra visibility.
- When touching form injection, ensure you preserve both `hook_form_alter()` and the after-build handler so the button remains robust to Metatag ordering.

Examples for quick reference
- To find where the button is added: search `openai_metatag_after_build` and `openai_metatag_generate_submit` in `openai_metatag.module`.
- To see JSON parsing of AI output: inspect `openai_metatag_generate_suggestions()` — the function strips fences, decodes JSON, and normalizes title/description/og fields.

What to avoid
- Do not assume linear form structure — Metatag uses nested groups; use the provided recursive helpers to find `['value']` elements rather than direct array indexing.

If anything is missing or unclear, tell me which area you'd like expanded (examples, more file pointers, or a checklist of debugging commands) and I will iterate.
