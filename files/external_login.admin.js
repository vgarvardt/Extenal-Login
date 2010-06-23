/* $Id$ */

/**
 * Move a provider in the providers table from one state to another via select list.
 *
 * This behavior is dependent on the tableDrag behavior, since it uses the
 * objects initialized in that behavior to update the row.
 */
Drupal.behaviors.external_login_providers_drag = function(context) {
  var table = $('table#external_login_providers');
  var tableDrag = Drupal.tableDrag.external_login_providers; // Get the blocks tableDrag object.

  // Add a handler for when a row is swapped, update empty regions.
  tableDrag.row.prototype.onSwap = function(swappedRow) {
    checkEmptyRegions(table, this);
  };

  // Add a handler so when a row is dropped, update fields dropped into new state.
  tableDrag.onDrop = function() {
    dragObject = this;
    if ($(dragObject.rowObject.element).prev('tr').is('.region-message')) {
      var regionRow = $(dragObject.rowObject.element).prev('tr').get(0);
      var regionName = regionRow.className.replace(/([^ ]+[ ]+)*region-([^ ]+)-message([ ]+[^ ]+)*/, '$2');
      var regionField = $('select.external_login_provider-region-select', dragObject.rowObject.element);
      var weightField = $('select.external_login_provider-weight', dragObject.rowObject.element);
      var oldRegionName = weightField[0].className.replace(/([^ ]+[ ]+)*openid_provider-weight-enabled-([^ ]+)([ ]+[^ ]+)*/, '$2');

      if (!regionField.is('.external_login_provider-region-' + regionName)) {
        regionField.removeClass('external_login_provider-region-enabled-' + oldRegionName).addClass('external_login_provider-region-enabled-' + regionName);
        weightField.removeClass('external_login_provider-weight-enabled-' + oldRegionName).addClass('external_login_provider-weight-enabled-' + regionName);
        regionField.val(regionName);
      }
    }
  };

  // Add the behavior to each state select list.
  $('select.external_login_provider-region-select:not(.external_login_providerregionselect-processed)', context).each(function() {
    $(this).change(function(event) {
      // Make our new row and select field.
      var row = $(this).parents('tr:first');
      var select = $(this);
      tableDrag.rowObject = new tableDrag.row(row);

      // Find the correct region and insert the row as the first in the state.
      $('tr.region-message', table).each(function() {
        if ($(this).is('.region-' + select[0].value + '-message')) {
          // Add the new row and remove the old one.
          $(this).after(row);
          // Manually update weights and restripe.
          tableDrag.updateFields(row.get(0));
          tableDrag.rowObject.changed = true;
          if (tableDrag.oldRowElement) {
            $(tableDrag.oldRowElement).removeClass('drag-previous');
          }
          tableDrag.oldRowElement = row.get(0);
          tableDrag.restripeTable();
          tableDrag.rowObject.markChanged();
          tableDrag.oldRowElement = row;
          $(row).addClass('drag-previous');
        }
      });

      // Modify empty states with added or removed fields.
      checkEmptyRegions(table, row);
      // Remove focus from selectbox.
      select.get(0).blur();
    });
    $(this).addClass('external_login_providerregionselect-processed');
  });

  var checkEmptyRegions = function(table, rowObject) {
    $('tr.region-message', table).each(function() {
      // If the dragged row is in this region, but above the message row, swap it down one space.
      if ($(this).prev('tr').get(0) == rowObject.element) {
        // Prevent a recursion problem when using the keyboard to move rows up.
        if ((rowObject.method != 'keyboard' || rowObject.direction == 'down')) {
          rowObject.swap('after', this);
        }
      }
      // This state has become empty
      if ($(this).next('tr').is(':not(.draggable)') || $(this).next('tr').size() == 0) {
        $(this).removeClass('region-populated').addClass('region-empty');
      }
      // This state has become populated.
      else if ($(this).is('.region-empty')) {
        $(this).removeClass('region-empty').addClass('region-populated');
      }
    });
  };
};
