<?php // $Id: block.tpl.php,v 1.3 2008/09/09 13:14:22 jmburnz Exp $
/**
 * @file
 *  block.tpl.php
 *
 * Theme implementation to display a block.
 *
 * @see template_preprocess()
 * @see template_preprocess_block()
 */
?>
<div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="block <?php print $block_classes; ?>">
  <div class="block-inner">

  <?php if (!empty($block->subject)): ?>
    <h2 class="block-title"><?php print $block->subject; ?></h2>
  <?php endif; ?>

    <div class="block-content">
      <div class="block-content-inner">
        <?php print $block->content; ?>
      </div>
    </div>
  </div>
</div> <!-- /block -->