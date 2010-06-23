<?php
// $Id$

/**
 * @file
 * Template for providers manipulations.
 *
 * Builds table with available providers and allows Enable/Disable providers and change providers weight
 * using Drag&Drop. Base of this template is taken from Block module template
 */

/**
 * Available variables:
 * - $regions - regions (provider states)
 * - $providers - providers list assoc array with region as key and providers array in this state
 * - $form - all another form elements that should exists on form
 */
?>

<?php
// add all required JavaScript for providers manipulation
drupal_add_js('misc/tableheader.js');
drupal_add_js(drupal_get_path('module', 'openid_ext') .'/files/openid_ext.admin.js');
foreach ($regions as $region => $title) {
  drupal_add_tabledrag('openid_providers', 'match', 'sibling', 'openid_provider-region-select', 'openid_provider-region-enabled-' . $region, NULL, FALSE);
  drupal_add_tabledrag('openid_providers', 'order', 'sibling', 'openid_provider-weight', 'openid_provider-weight-enabled-' . $region);
}
?>
<table id="openid_providers" class="sticky-enabled">
	<thead>
		<tr>
			<th><?php print t('Provider'); ?></th>
			<th><?php print t('Enabled'); ?></th>
			<th><?php print t('Weight'); ?></th>
		</tr>
	</thead>
	<tbody>
		<?php $row = 0; ?>
		<?php foreach ($regions as $region => $title): ?>
		<tr class="region region-<?php print $region?>">
			<td colspan="3" class="region"><?php print $title; ?></td>
		</tr>
		<tr class="region-message region-<?php print $region?>-message <?php print empty($providers[$region]) ? 'region-empty' : 'region-populated'; ?>">
			<td colspan="3"><em><?php print t('No providers in this state'); ?></em></td>
		</tr>
			<?php foreach ($providers[$region] as $pname => $data): ?>
		<tr class="draggable <?php print $row % 2 == 0 ? 'odd' : 'even'; ?>">
			<td><?php print $data->title; ?></td>
			<td><?php print $data->enable_select; ?></td>
			<td><?php print $data->weight_select; ?></td>
		</tr>
				<?php $row++; ?>
			<?php endforeach; ?>
		<?php endforeach; ?>
	</tbody>
</table>

<?php print $form; ?>