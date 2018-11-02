<?php

 /**
  * @file
  * Provide from definition for student module.
  */

 /**
  * Implements hook_form().
  */
function student_form($form, &$form_state) {
  $form['st_fnm'] = array(
    '#type' => 'textfield',
    '#title' => t('First Name:'),
    '#size' => 25,
    '#required' => TRUE,
  );
  $form['st_lnm'] = array(
    '#type' => 'textfield',
    '#title' => t('Last Name:'),
    '#size' => 25,
    '#required' => TRUE,
  );
  $form['st_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email:'),
    '#size' => 25,
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Check In'),
  );
  return $form;
}

/**
 * Process student_form form submissions. Event Handler.
 */
function student_form_submit($form, &$form_state) {
  $st_ID = db_insert('student')
    ->fields(array(
      'st_fnm' => check_plain($form_state['values']['st_fnm']),
      'st_lnm' => check_plain($form_state['values']['st_lnm']),
      'st_email' => check_plain($form_state['values']['st_email']),
    ))
    ->execute();
  drupal_set_message(t('Entry has been added.'));
}

/**
 * Process List of Student.
 */
function student_list() {
  $query = db_select('student', 'st');
  $query
    ->fields('st', array('st_id', 'st_fnm', 'st_lnm', 'st_email'))
    ->range(0, 50)
    ->orderby('st.st_id');
  $results = $query
    ->execute();
  $header = array(t('ID'),
    t('First Name'),
    t('Last Name'),
    t('E-mail'),
    t('Delete'),
    t('Edit'),
  );
  $rows = array(); /*values pulling database*/
  foreach ($results as $result) {
    $rows[] = array(
      check_plain($result->st_id),
      check_plain($result->st_fnm),
      check_plain($result->st_lnm),
      check_plain($result->st_email),
      l(t('Delete'), 'formlist/' . $result->st_id . '/delete'),
      l(t('Edit'), 'formlist/' . $result->st_id . '/edit'),
    );
  }
  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Process student_form delete submissions.
 */
function student_form_delete() {
  $id = arg(1);
  $num_deleted = db_delete('student')
    ->condition('st_id', $id)
    ->execute();
  if ($num_deleted) {
    drupal_set_message(t('entry has been deleted.'));
    drupal_goto("formlist");
  }
  else {
    drupal_set_message(t('error in query'));
  }
}

/**
 * Selecting from database.
 */
function student_edit_form($form, &$form_state) {
  $form = array();
  $id = arg(1);
  $result = db_select('student', 'st')
    ->fields('st', array('st_fnm', 'st_lnm', 'st_email')
    )
    ->condition('st_id', $id)
    ->execute()
    ->fetchObject();

  $form['st_fnm'] = array(
    '#type' => 'textfield',
    '#title' => t('Name:'),
    '#size' => 50,
    '#required' => TRUE,
    '#value' => ($result->st_fnm),
  );
  $form['st_lnm'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name:'),
    '#size' => 30,
    '#required' => TRUE,
    '#value' => ($result->st_lnm),
  );
  $form['st_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email :'),
    '#size' => 30,
    '#required' => TRUE,
    '#value' => ($result->st_email),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Process student_form form submissions.
 */
function student_edit_form_submit($form, &$form_state) {
  $id = arg(1);
  $num_edited = db_update('student')
    ->fields(array(
      'st_fnm' => check_plain($form_state['input']['st_fnm']),
      'st_lnm' => check_plain($form_state['input']['st_lnm']),
      'st_email' => check_plain($form_state['input']['st_email']),
    ))
    ->condition('st_id', $id, NULL, 'IS NOT NULL')
    ->execute();
  drupal_set_message(t('Entry has been updated.')
  );
}
