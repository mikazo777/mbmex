#!/bin/sh

sudo systemctl daemon-reload
sudo systemctl restart mbmex
systemctl status mbmex
