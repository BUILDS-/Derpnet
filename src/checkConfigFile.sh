#!/bin/bash

if [ -e "irc.conf" ]
then 
  echo "irc.conf exists."
else 
  echo "irc.conf is missing. Copying over irc.conf_EXAMPLE for now."
  if [ -e "irc.conf_EXAMPLE" ] 
	then
	  cp irc.conf_EXAMPLE irc.conf 
	  echo "OK, you should be all set."
	else 
    echo "irc.conf_EXAMPLE is missing too - try a fresh pull."
	fi
fi	
