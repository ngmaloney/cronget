Description:
------------
This module retrieves remote data via drupal_http_request and saves it. It is
executed via the cron hook. The data is saved in a local table and can also
be saved as a node or a block. There is also callback functionality
so the remote data can be manipulated after retrieval.

It was developed by (and for) Bentley College to keep components of their drupal
site synced with their other web properties. It borrows heavily from the
aggregator module.

Element Overview:
------------------
Title: The job title.

URL: The remote URL to retrieve. The module passes this variable to
drupal_http_request.

Job Type: This indicates how to handle data on each execution.
  -Standard: Stores data in one row after every execution.
  -Streaming: Stores data in a new row after every execution.

Save As: Will optionally save the job as a node or block.

Description: A description of the job so you remember what it is ;)

Callback: PHP code that will execute after job completion. If you wished
to do some screen scraping or replacing it would happen here. The
following variables will be helpful:

$data['jid] : The Job ID that was executed.
$data['content'] : The remote content that was fetched.


Code Contributions
------------------
ngmaloney



Todo:
------------------
Refine Permissions
Refine Menu Caching