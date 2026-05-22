(function (window, document, $, Drupal) {
  'use strict';

  function getPreviewRoot() {
    var el = document.getElementById('me-preview') || document.getElementById('me_preview');
    if (!el) {
      return null;
    }
    if (el.tagName && el.tagName.toLowerCase() === 'iframe') {
      try {
        return el.contentDocument || (el.contentWindow && el.contentWindow.document) || null;
      }
      catch (e) {
        return null;
      }
    }
    return el;
  }

  function extractPreviewText() {
    var root = getPreviewRoot();
    if (!root) {
      return '';
    }
    var body = root.body || root;
    var text = body && body.textContent ? body.textContent : '';
    text = String(text || '').replace(/\s+/g, ' ').trim();
    if (text.length > 50000) {
      text = text.substring(0, 50000);
    }
    return text;
  }

  function setPreviewHiddenValue(text) {
    var input = document.querySelector('input[name="ai_metatag_preview_text"]');
    if (!input) {
      return;
    }
    input.value = text || '';
    if (typeof $ === 'function') {
      $(input).trigger('change');
    }
  }

  function bind(context) {
    var $ctx = typeof $ === 'function' ? $(context) : null;
    var $buttons = $ctx ? $ctx.find('.ai-metatag-generate-button') : $('.ai-metatag-generate-button');

    $buttons.each(function () {
      var el = this;
      if (el._aiMetatagPreviewBound) {
        return;
      }
      el._aiMetatagPreviewBound = true;
      el.addEventListener('click', function () {
        setPreviewHiddenValue(extractPreviewText());
      });
    });
  }

  Drupal = Drupal || window.Drupal || {};
  Drupal.behaviors = Drupal.behaviors || {};
  Backdrop.behaviors.aiMetatagPreviewExtract = {
    attach: function (context) {
      bind(context || document);
    }
  };
})(window, document, (window.jQuery || window.$), window.Drupal);
