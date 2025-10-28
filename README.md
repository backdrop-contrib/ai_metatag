# OpenAI Metatag Helper

## What does it do?
OpenAI Metatag Helper adds a "Generate Metatags with OpenAI" button to Backdrop node edit forms and an option to auto-generate missing meta tags on save. It uses the site's configured OpenAI integration to produce SEO-friendly values (title, description, open-graph fields, keywords) and populates Metatag fields where appropriate.

This module is small and focused: it integrates with the `openai` module for API access, the `metatag` module for field rendering, and the `key` module for secure API key storage. It never overwrites existing tag values — only empty text-like metatag fields are populated.

## Getting started
1. Enable the required modules: `openai`, `metatag`, and `key`.
2. Enable `OpenAI Metatag Helper` (the `openai_metatag` module).
3. Ensure Metatag fields are available on the content types you want to use.
4. Configure OpenAI credentials using the `key` or `openai` module as usual.
5. Visit Admin -> Configuration -> Open AI -> OpenAI Metatag Assistant (`admin/config/openai/metatag`) to:
   - Toggle per-content-type enablement.
   - Turn on `Auto-generate meta tags on save` (module default on install).
   - Select the AI model and `AI max tokens` used for generation.
6. Give users the permission `Generate metatags with openai` (and ensure they have `Edit meta tags` or `Administer meta tags`).

## How to use
- Edit a node that has Metatag support.
- Click the "Generate Metatags with OpenAI" button in the Metatag fieldset to populate available text fields via AJAX.
- Review and save — the module does not overwrite non-empty metatag values.
- If `Auto-generate meta tags on save` is enabled, missing text-like tags will be filled during `hook_node_presave()`.

## Project-specific details and patterns
- Admin page: `openai_metatag_settings_form()` in `openai_metatag.module` (path: `admin/config/openai/metatag`).
- Button injection: the module attaches both `hook_form_alter()` and an `#after_build` handler (`openai_metatag_after_build`) to ensure the Generate button is added after Metatag builds its UI.
- Text extraction: `openai_metatag_collect_text_from_values()` and `openai_metatag_collect_text_from_entity()` recursively gather content to provide context to the model (limits are applied to avoid token overuse).
- Model selection: module-level options fall back to `openai.settings` when not set; see `options.ai_model` and `options.ai_max_tokens` in the settings form.
- Permission: the custom permission is `generate metatags with openai` (see `openai_metatag_permission()`).
- Logging/diagnostics: the module uses Backdrop `watchdog()` calls for debug/error information if generation or form injection fails.

## Requirements
- Backdrop CMS 1.x
- Modules: `openai`, `metatag`, `key` (declared in `openai_metatag.info`).

## Troubleshooting
- If the Generate button is not visible: confirm the Metatag fieldset is enabled for the content type and the current user has `Edit meta tags` and `Generate metatags with openai` permissions.
- If generation fails: check that the OpenAI API key is configured (the module reads keys via the `key` integration) and inspect logs for `watchdog()` entries.

## Files of interest
- `openai_metatag.module` — main logic (form alters, AJAX, generation and presave behavior).
- `openai_metatag.install` — install/update hooks (default settings).
- `openai_metatag.info` — module metadata and declared dependencies.

## Installation

- Install this module using the official [Backdrop CMS instructions](https://backdropcms.org/user-guide/modules).

## Issues

Bugs and feature requests should be reported in the [Issue Queue](https://github.com/backdrop-contrib/openai_metatag/issues).


## Current Maintainer

[Justin Keiser](https://github.com/keiserjb)

## Credits

Inspired by the [Metatag AI](https://www.drupal.org/project/metatag_ai) module in Drupal.

## License
This project is GPL v2 software. See the `LICENSE.txt` file in this directory for the full text.
