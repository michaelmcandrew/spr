/*
Copyright (c) 2003-2011, CKSource - Frederico Knabben. All rights reserved.
For licensing, see LICENSE.html or http://ckeditor.com/license
*/

CKEDITOR.editorConfig = function( config )
{
	// Define changes to default configuration here. For example:
	// config.language = 'fr';
	// config.uiColor = '#AADC6E';

  /* DrupalFiltered toolbar below modified to be similar to earlier FCKeditor version */
  config.toolbar_DrupalFiltered = [
    ['Source'],
    ['Cut','Copy','Paste','PasteText','PasteFromWord'],
    ['Undo','Redo','Find','Replace','-','SelectAll','RemoveFormat'],
    ['Link','Unlink','Anchor','LinkToNode', 'LinkToMenu'],
    ['Image','Flash','Table','HorizontalRule','Smiley','SpecialChar'],
    '/',
    ['Format'],
    ['Bold','Italic','Underline','Strike','-','Subscript','Superscript'],
    ['NumberedList','BulletedList','-','Outdent','Indent','Blockquote'],
    ['JustifyLeft','JustifyCenter','JustifyRight','JustifyBlock'],
    ['DrupalBreak', 'DrupalPageBreak'],
    ['Maximize', 'ShowBlocks']
   ];
   config.toolbar = 'DrupalFiltered';
   config.width = '100%';
};
