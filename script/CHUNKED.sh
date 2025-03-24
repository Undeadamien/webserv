#!/bin/env bash

IP="localhost"
PORT=3434

RATE="10b"

DATA="Hello world this is a chunked request"

echo "Testing for port: ${PORT}"
printf "${DATA}" |
	curl -X POST "http://${IP}:${PORT}" \
		-H "Transfer-Encoding: chunked" \
		--data-binary @- \
		--limit-rate "${RATE}"
