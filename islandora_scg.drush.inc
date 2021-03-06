<?php

/**
 * @file
 * Drush file for the Islandora Sample Content Generator.
 */

/**
 * Implements hook_drush_command().
 */
function islandora_scg_drush_command() {
  $items = array();
  $items['islandora_scg_load'] = array(
    'aliases' => array('iscgl'),
    'description' => 'Generate and load sample Islandora content',
    'examples' => array(
      'drush iscgl --user=admin --quantity=20 --content_model=islandora:sp_basic_image --parent=islandora:sp_basic_image_collection --namespace=foo',
      'drush iscgl --user=admin --content_model=islandora:sp_basic_image --parent=islandora:sp_basic_image_collection',
    ),
    'options' => array(
      'quantity' => array(
        'description' => 'The number of objects to load. Defaults to 5.',
      ),
      'content_model' => array(
        'description' => 'The content models to assign to the objects.',
        'required' => TRUE,
      ),
      'parent' => array(
        'description' => 'The collection to which the generated items should ' .
        'be added.',
        'required' => TRUE,
      ),
      'namespace' => array(
        'description' => 'Namespace of objects to create. ' .
        'Defaults to "islandora".',
      ),
      'pages' => array(
        'description' => 'For paged content (including PDFs), the number of ' .
        'pages in each sample object. Defaults to 4.',
      ),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  return $items;
}

/**
 * Generates and loads sample content.
 */
function drush_islandora_scg_load() {
  $options = array(
    'quantity' => drush_get_option('quantity', 5),
    'content_model' => drush_get_option('content_model'),
    'parent' => drush_get_option('parent'),
    'namespace' => drush_get_option('namespace', 'islandora'),
    'pages' => drush_get_option('pages', 4),
  );

  // Map content models to classes. If you add new content
  // models, register them here.
  $registry = array(
    'islandora:sp_basic_image' => 'IslandoraSampleBasicImageGenerator',
    'islandora:sp_large_image_cmodel' => 'IslandoraSampleLargeImageGenerator',
    'islandora:sp_pdf' => 'IslandoraSamplePdfGenerator',
  );

  drush_print("Starting to generate sample content...");
  $generator = new $registry[$options['content_model']]($options);
  $generator->createDataDir();
  $generator->generateContent();

  drush_print("Starting to load sample content...");
  $generator->loadContent();
  $generator->removeDataDir();
}
