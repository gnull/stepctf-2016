#!/usr/bin/env perl

use strict;
use Data::Dumper;

sub info {
    local $\;
    local $,;
    $\ = "\n";
    $, = ", ";
    print STDERR "@_";
}

# path handler functions
my %contents;

$contents{'/'} = sub {
    return << "EOF";
<html>
<head>
<title> The most awesome ping utility! </title>
</head>

<body>

Welcome to the most awesome ping utility!

<br>

Ping:

<form action="/ping" method="get">
  <input type="text" name="host" value="google.com"/>
  <select name="count">
    <option value="1">once</option>
    <option value="2">twise</option>
    <option value="3">three times</option>
  </select>
  <input type="submit" value="Go!"/>
</form>

</body>
</html>
EOF
};

$contents{'/ping'} = sub {
    my %params = @_;
    my $result;
    if ($params{host} =~ m:[0-9a-zA-Z\.]+: ) {
	$result = `ping $params{host} -c $params{count} 2>&1` ;
    } else {
	$result = "You have been caught. Go and pwn someone else."
    }
    my $err = '';
    $err = "Error: $?<br>";
    return << "EOF";
<html>
<head>
<title>The most awesome ping results</title>
</head>
<body>

$err

Results: <br>

<pre>
$result
</pre>

</body>
</html>
EOF
};

sub tx {
    my $content = shift;
    $content .= "\n <!-- use Perl or die; --> \n";
    my $content_length = length($content);
    print << "EOF";
HTTP/1.1 200 OK\r
Content-Length: $content_length\r
Content-Type: text/html\r
Connection: close
\r
$content
EOF
}

sub err {
    local $,;
    $, = ' ';
    info "err: @_";
    my $resp = << "EOF";
<html>
<title>@_</title>
<body>
@_
</body>
</html>
EOF
    tx $resp;
    exit 1;
}

sub get_url {
    my $start = <>;
    info $start;
    my ($method, $url, $version) = split m/\s+/, $start;
    info $method, $url, $version;
    err "uknown method: $method" if $method ne 'GET';
    return $url;
}

sub main {
    my $url = get_url;

    my ($path, $request) = split m/\?/, $url;
    my %params = split m/[&=]/, $request;

    if (exists $contents{$path}) {
	my $reply = &{$contents{$path}} (%params);
	tx $reply;
    } else {
	tx << "EOF";
<html>
<title> No such file </title>
<body>No such file or directory: $path</body>
</html>
EOF
    }
}

main;



