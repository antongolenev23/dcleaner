<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8" />
<meta name="viewport" content="width=device-width, initial-scale=1" />
<title>dcleaner README</title>
<style>
  body { font-family: Arial, sans-serif; max-width: 800px; margin: auto; padding: 20px; line-height: 1.6; background: #f9f9f9; }
  h1, h2, h3 { color: #2c3e50; }
  pre { background: #2d2d2d; color: #f8f8f2; padding: 12px; border-radius: 5px; overflow-x: auto; }
  code { background: #eee; padding: 2px 4px; border-radius: 3px; }
  blockquote { color: #555; border-left: 4px solid #ccc; margin: 1em 0; padding-left: 10px; font-style: italic; background: #f0f0f0; }
  hr { border: 0; border-top: 1px solid #ccc; margin: 2em 0; }
  a { color: #2980b9; text-decoration: none; }
  a:hover { text-decoration: underline; }
  .warning { background: #fff3cd; padding: 10px; border-left: 4px solid #ffc107; margin: 1em 0; }
  .note { background: #e7f5ff; padding: 10px; border-left: 4px solid #4dabf7; margin: 1em 0; }
</style>
</head>
<body>

<h1>dcleaner</h1>

<blockquote>A command-line utility to scan and clean up unnecessary files: temporary files, cache (including thumbnails), empty folders, and unused directories. Designed to be <strong>fast</strong>, <strong>safe</strong>, and <strong>simple to use</strong>.</blockquote>

<hr />

<h2>Purpose</h2>

<p><code>dcleaner</code> helps you keep your system clean by detecting and removing:</p>

<ul>
  <li><strong>Temporary files</strong> (<code>*.tmp</code>, backup files, editor swap files, etc.)</li>
  <li><strong>Cache directories and preview/thumbnail files</strong> (e.g., browser cache, <code>~/.cache/</code>, <code>Thumbs.db</code>, etc.)</li>
  <li><strong>Empty folders</strong></li>
  <li><strong>Inactive directories</strong> (not accessed in N days)</li>
</ul>

<div class="note">
  <h3>ℹ️ Safety Notes</h3>
  <p>By default, <code>dcleaner</code> prioritizes safety:</p>
  <ul>
    <li>Deletion uses the system trash/recycle bin (unless <code>--force</code> is specified).</li>
    <li>Files locked by other processes are skipped.</li>
  </ul>
</div>

<div class="warning">
  <h3>⚠️ Potential Risks</h3>
  <p>Some files/folders require caution:</p>
  <ul>
    <li><strong>Temporary files (*.tmp)</strong>: May be in use by running applications. <em>Recommendation:</em> Skip files modified in the last 24 hours.</li>
    <li><strong>Browser cache</strong>: Clearing may slow down subsequent website loads.
    <li><strong>Empty folders</strong>: Some apps (e.g., <code>node_modules/</code>) expect them. <em>Recommendation:</em> Allow exclude lists.</li>
    <li><strong>System directories</strong> (e.g., <code>/tmp/</code>, <code>~/Library/Caches/</code> on macOS). <em>Recommendation:</em> Avoid unless explicitly requested.</li>
  </ul>
</div>

<hr />

<h2>Usage</h2>

<p><code>dcleaner</code> operates in two modes: <strong>analysis</strong> (scan only) and <strong>deletion</strong> (cleanup).</p>
<h3>1. Analyze Mode</h3>

<p>Scans directories and reports statistics without deleting anything:</p>

<pre><code>analyze /path/to/dir1 /path/to/dir2 [--inactive-days 30] [--temp] [--cache] [--empty] [--inactive] [--exclude node_modules,.git] [--min-age 24h]</code></pre>

<p><strong>Output example:</strong></p>
<pre><code>Scan completed. Found:
- 124 temporary files (1.2 GB)
- 45 cache directories (580 MB)
- 12 empty folders
- 7 inactive folders (last accessed >30 days ago)</code></pre>

<div class="note">
  <p><strong>ℹ️ Note 1:</strong> All category flags (<code>--temp</code>, <code>--cache</code>, <code>--empty</code>, <code>--inactive</code>) are supported in <strong>analyze</strong> mode. Use them to restrict the analysis to specific types of files or folders.</p>
  <p><strong>ℹ️ Note 2:</strong> If you run <code>analyze</code> without any paths, <code>dcleaner</code> will scan the entire file system starting from root (<code>/</code> or system drive). This may take significantly more time. </p>
  <p><strong>ℹ️ Note 3:</strong> Use <code>--exclude</code> to skip certain subpaths (e.g. <code>--exclude node_modules</code>).</p>
  <p><strong>ℹ️ Note 4:</strong> Use <code>--min-age</code> to ignore files modified more recently than the specified age (e.g. <code>--min-age 12h</code>, <code>--min-age 2d</code>).</p>
</div>

<h3>2. Delete Mode</h3>

<p>Removes files based on prior analysis or direct paths:</p>

<pre><code>dcleaner delete [--temp] [--cache] [--empty] [--inactive] [--force] [--exclude node_modules,.git] [--min-age 24h]</code></pre>

<p>If you run <code>delete</code> <strong>without any flags</strong> after a successful <code>analyze</code>, <code>dcleaner</code> will delete <em>everything found in the most recent analysis</em>.</p>

<div class="note">
  <p><strong>Key safeguards:</strong></p>
  <ul>
    <li>Requires explicit flags (e.g., <code>--temp</code>) to delete specific categories, unless used immediately after <code>analyze</code>.</li>
    <li>Defaults to trash/recycle bin (use <code>--force</code> for permanent deletion).</li>
    <li>Supports <code>--exclude</code> and <code>--min-age</code> to fine-tune what is deleted, even after prior analysis.</li>
    <li>Shows a warning if run without prior analysis or paths.</li>
  </ul>
</div>



<h3>3. Interactive Mode</h3>

<p>Launch with no arguments for a step-by-step interface:</p>
<pre><code>$ dcleaner
dcleaner&gt; analyze ~/Downloads --inactive-days 60
dcleaner&gt; delete --temp --cache
dcleaner&gt; exit</code></pre>

<hr />

<h2>Safety Features</h2>

<ul>
  <li><strong>Exclude lists</strong>: Ignore specific paths via config file or CLI (<code>--exclude node_modules,.git</code>).</li>
  <li><strong>File age filters</strong>: Skip recent files (<code>--min-age 24h</code>).</li>
</ul>

<hr />

<h2>Installation</h2>

<hr />

<h2>License</h2>

<p>MIT License — see the LICENSE file.</p>

<hr />

<h2>Author</h2>

<p>Made by <a href="https://github.com/yourusername">Anton Golenev</a> with love.</p>

</body>
</html>