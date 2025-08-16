<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8" />
<meta name="viewport" content="width=device-width, initial-scale=1" />
</head>
<body>

<h1>dcleaner</h1>

<blockquote>A command-line utility to scan and clean up unnecessary files: unused files and empty directories. Designed to be <strong>fast</strong>, <strong>safe</strong>, and <strong>simple to use</strong>.</blockquote>

<hr />

<h2>Purpose</h2>

<p><code>dcleaner</code> helps you keep your system clean by detecting and removing:</p>

<ul>
  <li><strong>Inactive directories</strong> (not accessed in N days)</li>
  <li><strong>Empty folders</strong></li>
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
  <h3>⚠️ Critical Usage Notes</h3>
  <ul>
    <li><strong>Linux-only:</strong> this tool targets Linux systems. </li>
    <li><strong>Do not point it at root folders</strong> (e.g., <code>/</code>, system-wide mount points). Scanning the entire filesystem can take a very long time and may have unintended consequences.</li>
    <li><strong>No absolute guarantees:</strong> while designed to be safe, there is <em>no guarantee</em> that important files won’t be deleted if you misconfigure filters/paths. Review the output of <code>analyze</code> carefully before deletion.</li>
    <li><strong>Best practice:</strong> use <code>dcleaner</code> for <em>narrow, local</em> directories where you understand the contents (e.g., a reports folder, downloads, project scratch space), rather than system or shared roots.</li>
  </ul>
</div>

<h2>Usage Guide</h2>

<h2>Build / Installation</h2>

<p>To build <code>dcleaner</code> from source, run the provided build script:</p>

<pre><code>./build.sh
</code></pre>

<p>This script will:</p>
<ul>
  <li>Create a <code>build/</code> directory (if not already present).</li>
  <li>Configure the project using <code>cmake</code>.</li>
  <li>Compile the source code in <strong>Release</strong> mode by default.</li>
</ul>

<p>After a successful build, the <code>dcleaner</code> binary will be available in <code>build/</code>.</p>

<p>To install it into your local user directory (<code>~/.local/bin</code>):</p>
<pre><code>cmake --install build --prefix ~/.local
</code></pre>

<p>Make sure <code>~/.local/bin</code> is in your <code>$PATH</code> so you can run <code>dcleaner</code> from anywhere.</p>


<h3>General Notes</h3>
<ul>
  <li>At least one of <code>--empty</code> or <code>--inactive</code> must be specified, otherwise no action is performed.</li>
  <li>Exclusion patterns support globbing (see section <strong>Glob patterns</strong>).</li>
</ul>

<h3>Commands</h3>

<h4><code>analyze &lt;paths...&gt; [options]</code></h4>
<p>Analyze specified directories or files.</p>
<p>Searches for:</p>
<ul>
  <li>Files that have not been used for more than the specified number of days (default: 30 days if <code>--inactive</code> is set without <code>--inactive-days</code>).</li>
  <li>Empty directories (if <code>--empty</code> flag is provided).</li>
</ul>

<p><strong>Required:</strong></p>
<ul>
  <li><code>&lt;paths...&gt;</code> — one or more paths to analyze.</li>
</ul>

<p><strong>Options:</strong></p>
<ul>
  <li><code>--empty</code> — search for empty directories.</li>
  <li><code>--inactive</code> — search for inactive files.</li>
  <li><code>--inactive-days=&lt;N&gt;</code> — inactivity threshold in days.</li>
  <li><code>--exclude=&lt;patterns&gt;</code> — comma-separated glob patterns to exclude.  
      Example: <code>**/main.{go,py},temp/*</code></li>
</ul>

<p><strong>Examples:</strong></p>
<pre><code>analyze /home/user/project --empty
analyze /home/user/photo /home/user/texts/ --inactive --inactive-days=90 --exclude=**/*.tmp
</code></pre>

<h4><code>delete &lt;paths...&gt; [options]</code></h4>
<p>Delete files or directories identified by a prior <code>analyze</code> command.  
You can also run <code>delete</code> directly, but then you must specify paths and options.</p>

<p><strong>Note:</strong> at least one of <code>--empty</code> or <code>--inactive</code> must be specified when running <code>delete</code> without a prior analyze.</p>

<p><strong>Required (if not preceded by analyze):</strong></p>
<ul>
  <li><code>&lt;paths...&gt;</code> — one or more paths to delete.</li>
</ul>

<p><strong>Options:</strong></p>
<ul>
  <li><code>--empty</code> — same as in analyze.</li>
  <li><code>--inactive</code> — same as in analyze.</li>
  <li><code>--inactive-days=&lt;N&gt;</code> — same as in analyze.</li>
  <li><code>--exclude=&lt;patterns&gt;</code> — same as in analyze.</li>
  <li><code>--force</code> — permanently delete without moving to trash.</li>
</ul>

<p><strong>Examples:</strong></p>
<pre><code>delete --force
delete /home/user/old_logs /home/user/photo --inactive --inactive-days=60
</code></pre>

<h4><code>help</code></h4>
<p>Show help message.</p>

<h4><code>exit</code></h4>
<p>Exit the program.</p>

<hr />

<h3>Glob patterns</h3>
<p>Globs are simplified matching patterns for file/folder names.</p>

<p><strong>Supported special symbols:</strong></p>
<ul>
  <li><code>*</code> — matches zero or more characters in a single path segment.</li>
  <li><code>**</code> — matches zero or more path segments recursively.</li>
  <li><code>?</code> — matches exactly one character.</li>
  <li><code>[abc]</code> — matches one character from the set (a, b, or c).</li>
  <li><code>[a-z]</code> — matches one character in the range a–z.</li>
  <li><code>[!abc]</code> — matches any single character <strong>not</strong> in the set.</li>
  <li><code>{a,b,c}</code> — matches any of the listed alternatives.</li>
</ul>

<p><strong>Examples:</strong></p>
<pre><code>*.txt                   — matches all .txt files in current directory
**/*.log                — matches all .log files in all subdirectories
/home/*/.bashrc         — matches .bashrc in any user’s home directory
/var/**/log?.txt        — matches log1.txt, log2.txt, etc., anywhere under /var
/tmp/{a,b}/file         — matches /tmp/a/file or /tmp/b/file
</code></pre>
