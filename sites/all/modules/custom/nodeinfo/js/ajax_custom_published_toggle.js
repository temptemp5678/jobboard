/**
 *
 */
jQuery(document).ready(function() {
  // disable
  jQuery('.custom-published-wrapper input').change(function () {
    // if checkbox is checked -- "true", unchecked -- "undefined"
    if (jQuery(this.checked)[0]) {
      jQuery('#edit-options .form-item-status input').prop('checked', true);
    }
    else {
      jQuery('#edit-options .form-item-status input').prop('checked', false);
    }
  });



});
