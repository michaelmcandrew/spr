<?php
// $Id: weblinks-link.tpl.php,v 1.1.2.13 2009/07/03 21:22:50 nancyw Exp $
?>
<div class="weblinks weblinks-item weblinks-link-<?php print $variables[0]->nid;?>">
  <?php
  if ($teaser && variable_get('weblinks_show_title', TRUE)) {
    print '<h2 class="title">'. $title .'</h2>';
  }
?>

<?php print $link_status; ?>
<?php
  if (!$teaser || variable_get('weblinks_show_url', TRUE)) {
    print $link;
  }
?>

<div class="weblinks-body">
<?php print $weblinks_body; ?>
</div><!--class="weblinks-body"-->

<?php
  if (isset($pagerank)) {
    print '<div class="weblinks-rank">';
    print t('Google page rank: !pr, Alexa traffic rank: !alexa &nbsp; (as of !date)',
      array('!pr' => $pagerank, '!alexa' => number_format($alexa, 0), '!date' => $rank_checked));
    print '</div><!--class="weblinks-rank"-->';
  }

  if (isset($click_count) && $click_count > 0) {
    print '<div class="weblinks-click-stats">'. t('Clicked !count times. Last clicked !last.', array('!count' => $click_count, '!last' => $last_click)) .'</div><!--class="weblinks-click-stats"-->';
  }
?>
</div><!--class="weblinks weblinks-item weblinks-link-<?php print $variables[0]->nid;?>"-->
