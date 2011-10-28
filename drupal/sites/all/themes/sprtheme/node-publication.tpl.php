<?php
// $Id: node.tpl.php,v 1.4 2008/09/15 08:11:49 johnalbin Exp $

/**
 * @file node.tpl.php
 *
 * Theme implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: Node body or teaser depending on $teaser flag.
 * - $picture: The authors picture of the node output from
 *	 theme_user_picture().
 * - $date: Formatted creation date (use $created to reformat with
 *	 format_date()).
 * - $links: Themed links like "Read more", "Add new comment", etc. output
 *	 from theme_links().
 * - $name: Themed username of node author output from theme_user().
 * - $node_url: Direct url of the current node.
 * - $terms: the themed list of taxonomy term links output from theme_links().
 * - $submitted: themed submission information output from
 *	 theme_node_submitted().
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type, i.e. story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *	 teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $teaser: Flag for the teaser state.
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *	 main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 */
?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?>"><div class="node-inner">

	<?php print $picture; ?>

	<?php if (!$page): ?>
		<h2  class="title">
			<a href="<?php print $node_url; ?>" title="<?php print $title ?>"><?php print $title. ','; ?> by <?php print $node->field_author[0][view]; ?></a>
		</h2>
	<?php else: ?>
		<h3 class="title">
			By <?php print $node->field_author[0][view]; ?> <?php print $node->field_authorsurname[0][view]; ?>  <?php print $node->field_otherauthors[0][view]; ?>
		</h3>
	<?php endif; ?>

	<div class="content">

<?php print $node->field_image[0][view]; ?>

<?php print $node->content['body']['#value'] ?>

<?php
if ($teaser) {
print '<span class="more-link"><a href="'.$node_url.'">read more >></a></span>';
}
?>

<?php if (!$block): ?>
	<br/><?php print $node->field_reference [0][view]; ?><br/>
<?php endif; ?>

<?php if ($page): ?>

		<br/>
		<h3 class="title">
			Reviewed for the SPR by: <?php print $node->field_reviewedby [0][view]; ?>
		</h3>
		<br/>

		 <?php print $node->field_sprreview [0][view]; ?>

<?php endif; ?>

<br/><br/>

	</div>

</div>


<div class="clearer"><!----></div>
</div> <!-- /node-inner, /node -->

