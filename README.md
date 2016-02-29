<html>

<head>
<h1>ciphertool</h1>
<p>A tool to encrypt and decrypt your files.<br>
Tested on OpenBSD 5.9.</p>
</head>

<body>
<b>Features</b>
<ul>
	<li>Use pledge(2)</li>
	<li>No password request to encrypt</li>
	<li>One time password to decrypt your files (*.enc)</li>
	<li>Encrypting process remove the plaintext files</li>
</ul>
<b>How to install</b>
<pre><code>git clone https://github.com/wesley974/ciphertool
cd ciphertool
# Modify the PASSWORD and HASH in the source file ciphertool.c
make
make install</code></pre>
<b>Usage</b>
<pre><code>ciphertool file...</code></pre>
<b>Caveat</b>
<pre><p>Do not use ciphertool in production, it is not yet secure. The full OpenSSL command line is visible using ps(1).</p></pre>
</body>
</html>
