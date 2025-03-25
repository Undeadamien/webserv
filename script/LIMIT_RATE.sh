#!/bin/env bash

IP="localhost"
PORT=3434

DATA="Hello world this is a request request"

echo "Testing for port: ${PORT}"
printf "${DATA}" |
	curl -X POST "http://${IP}:${PORT}" \
		--data-binary @- \
		--trace-ascii output.log \
		--limit-rate "${RATE}"
