## Bigscreen Player

This version is: 1.0
Date: 30-6-2006

## Some info
This player was created for use at Drome langaming events, to present information on bigscreens
in a stylish and animated fashion. Development for this particular program started in april 2004.
It has currently undergone 15 beta versions. This is version 1.0 and probably the last one, since
coming the upcoming Drome event (13) is the last one.
It tries to connect to a server as configured in the config.bsml file. BSML stands for BigsSreen
Markup Language (yeah I know), which is pretty much HTML or SGL or whatever markup language. The
files downloaded from the server are all either BSML files, or media files (images and fonts). All
in all it pretty much resembles an internet browser.
The player can't do anything else than play, administration of the contents has to be done by third
party. For example, an intranet for remote editing of the pages. The pages are downloaded through HTTP
so pages can be generated dynamically by a webserver and some smart programming in ASP or PHP or whatever
floats your boat.
The following pages are required on the server :
	- a Cue file
	- All the page files as mentioned in the cue file
	
The cue file is basicly the playlist file, and it looks like this:
	
	<bsml>
		<page file="..." duration=x.y>
		<page ...>
		<page ...>
	</bsml>
	
The player parses this and queue's pages in order as mentioned in the cue file.
The more interesting stuff happens in page files. There are several design tags possible, the design
can then be canceled by closing tags. Just like normal HTML. However, there are a few things that's not
possible in HTML, and there are a LOT of things BSML can't do.
Since the possibilities of BSML are pretty limited I'll summarize them here :
	
	<bsml>
		<font face="..." style="..." color="..." size=x>
			This text has special font properties!
		</font>
		
		<img src="..." left=x.x top=y.y width=w.w height=h.h>
			Images don't need a closing tag.
			
		<margin left=x.x top=y.y right=r.r>
			The following text is made sure to fit between the margins.
			A bottom margin is useless.
		</margin>
		
		<fade delay=
	...