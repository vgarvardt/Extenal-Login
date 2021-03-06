/* $Id$ */

/**
 * Behaviors for External Login themes.
 */
Drupal.behaviors.external_login_usability = function() {
  // if current theme is Dropdown or Default it should have some settings
  if (Drupal.settings.external_login._default || Drupal.settings.external_login.dropdown) {
    if (Drupal.settings.external_login._default) {
      var settings = Drupal.settings.external_login._default;
    }
    else {
      var settings = Drupal.settings.external_login.dropdown;
    }

    $('#edit-provider').change(function() {
      var val = $(this).val();

      if (settings[val] && settings[val].title) {
        $('label', $('#edit-input-wrapper')).text(settings[val].title);
        // remove all classes and leave only default form item class
        $('#edit-input').attr('class', '').addClass('form-text');
        if (settings[val].icon) {
          $('#edit-input').addClass('with-icon').addClass($('#edit-provider').val());
        }
        $('#edit-input-wrapper').show();
      }
      else {
        $('#edit-input-wrapper').hide();
      }
    }).change();
  }

  // if current theme is Accordion it should have some settings
  if (Drupal.settings.external_login.accordion) {
    var accordion_element = $('#' + Drupal.settings.external_login.accordion.container_id);
    $('li', accordion_element).each(function() {
      var list_element = $(this);
      var header_element = $('.accordion-header', list_element);
      var content_element = header_element.next();

      list_element.css('list-style', Drupal.settings.external_login.accordion.list_style);
      list_element.css('padding', Drupal.settings.external_login.accordion.padding);
      list_element.css('margin', Drupal.settings.external_login.accordion.margin);
      header_element.addClass('accordion-header');
      content_element.addClass('accordion-content').empty();
      $('a', header_element).click(function() {
        if (!content_element.hasClass('accordion-content-active')) {
          $('.accordion-content-active', accordion_element).each(function() {
            var el = $(this);
            el.removeClass('accordion-content-active').slideUp('normal');
            el.parents('li:first').css('list-style', Drupal.settings.external_login.accordion.list_style);
          });
          list_element.css('list-style', Drupal.settings.external_login.accordion.list_style_active);

          var new_provider = $(this).attr('class');

          if (Drupal.settings.external_login.accordion[new_provider] && Drupal.settings.external_login.accordion[new_provider].title) {
            if (!$('#edit-input-wrapper', content_element).size()) {
              var input_element = $('#edit-input-wrapper');
              content_element.append(input_element);
              $('label', input_element).text(Drupal.settings.external_login.accordion[new_provider].title);
              input_element.show();
            }
          }
          var op_element = $('#edit-op');
          content_element.append(op_element);
          op_element.show();
          $('#edit-provider').val(new_provider);

          content_element.addClass('accordion-content-active');
          content_element.slideDown('normal');
        }
        return false;
      });
    });
    $('a', $('li.first', accordion_element)).click();
  }
};