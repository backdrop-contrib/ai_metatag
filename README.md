# AI Metatag Helper

## What does it do?
AI Metatag Helper adds a "Generate SEO with AI" button to Backdrop node edit forms and an option to auto-generate missing meta tags on save. It uses the site's configured AI integration to produce SEO-friendly values (title, description, open-graph fields, keywords) and populates Metatag fields where appropriate.

This module is small and focused: it integrates with the `ai` module for API access and the `metatag` module for field rendering. The manual Generate button updates matching text-like metatag fields with fresh AI suggestions, while the optional presave auto-generation only fills empty values.

## Getting started
1. Enable the required modules: `ai` and `metatag`.
2. Enable `AI Metatag Helper` (the `ai_metatag` module).
3. Ensure Metatag fields are available on the content types you want to use.
4. Configure your AI provider in the `ai` module as usual.
5. Visit Admin -> Configuration -> Open AI -> AI Metatag Assistant (`admin/config/ai/metatag`) to:
   - Toggle per-content-type enablement.
   - Turn on `Auto-generate meta tags on save` (module default on install).
   - Select the AI model and `AI max tokens` used for generation.
6. Give users the permission `Generate metatags with ai` (and ensure they have `Edit meta tags` or `Administer meta tags`).

## How to use
- Edit a node that has Metatag support.
- Click the "Generate SEO with AI" button in the Metatag fieldset to update available text fields via AJAX.
- Review and save. The manual button can replace existing text-like metatag values, while presave auto-generation only fills missing values.
- If `Auto-generate meta tags on save` is enabled, missing text-like tags will be filled during `hook_node_presave()`.

## Project-specific details and patterns
- Admin page: `ai_metatag_settings_form()` in `ai_metatag.module` (path: `admin/config/ai/metatag`).
- Button injection: the module attaches both `hook_form_alter()` and an `#after_build` handler (`ai_metatag_after_build`) to ensure the Generate button is added after Metatag builds its UI.
- Text extraction: `ai_metatag_collect_text_from_values()` and `ai_metatag_collect_text_from_entity()` recursively gather content to provide context to the model (limits are applied to avoid token overuse).
- Model selection: module-level options fall back to `ai.settings` when not set; see `options.ai_model` and `options.ai_max_tokens` in the settings form.
- Permission: the custom permission is `generate metatags with ai` (see `ai_metatag_permission()`).
- Logging/diagnostics: the module uses Backdrop `watchdog()` calls for debug/error information if generation or form injection fails.

## Requirements
- Backdrop CMS 1.x
- Modules: `ai`, `metatag`.

## Troubleshooting
- If the Generate button is not visible: confirm the Metatag fieldset is enabled for the content type and the current user has `Edit meta tags` and `Generate metatags with ai` permissions.
- If generation fails: check that an enabled provider and chat model are configured in `ai`, then inspect logs for `watchdog()` entries.

## Files of interest
- `ai_metatag.module` — main logic (form alters, AJAX, generation and presave behavior).
- `ai_metatag.info` — module metadata and declared dependencies.

## Installation

- Install this module using the official [Backdrop CMS instructions](https://backdropcms.org/user-guide/modules).

## Issues

Bugs and feature requests should be reported in the [Issue Queue](https://github.com/backdrop-contrib/ai_metatag/issues).


## Current Maintainer

[Justin Keiser](https://github.com/keiserjb)

## Credits

Inspired by the [Metatag AI](https://www.drupal.org/project/metatag_ai) module in Drupal.

- Developed with AI assistance.

## License
This project is GPL v2 software. See the `LICENSE.txt` file in this directory for the full text.
