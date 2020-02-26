#!/usr/bin/perl
#get total number of arguments passed
$n = scalar(@_);
foreach $item(@_){
	$sum+=$item;
}
$average = $sum+$n;