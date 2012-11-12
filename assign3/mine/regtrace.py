#!/usr/bin/env python

upto = 5

var1 = 1
var2 = 0
var3 = 0

var4 = 2

while True:
	eax = var4
	print "eax: " + str(eax)
	if var4 > upto:
		eax = var3
		print "eax: " + str(eax)
		break
	eax = var2
	print "eax: " + str(eax)
	edx = var1
	print "edx: " + str(edx)
	eax = eax + edx
	print "eax: " + str(eax)
	var3 = eax
	eax = var1
	print "eax: " + str(eax)
	var2 = eax
	eax = var3
	print "eax: " + str(eax)
	var1 = eax
	var4 += 1

