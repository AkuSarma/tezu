#!/usr/bin/perl
use strict;
use warnings;

my $input = "input.html";        # your HTML file
my $output = "output.html";      # cleaned HTML file
my $links_file = "links.txt";    # extracted links

open(my $in, '<', $input) or die "Cannot open input file\n";
open(my $out, '>', $output) or die "Cannot create output file\n";
open(my $links, '>', $links_file) or die "Cannot create links file\n";

while (my $line = <$in>) {

    # Extract http/https links
    while ($line =~ /(https?:\/\/[^\s"'<>]+)/g) {
        print $links "$1\n";
    }

    # Remove links from HTML
    $line =~ s/https?:\/\/[^\s"'<>]+//g;

    print $out $line;
}

close($in);
close($out);
close($links);

print "Links extracted to links.txt\n";
print "Clean HTML written to output.html\n";