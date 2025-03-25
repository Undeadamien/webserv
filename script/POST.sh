#!/bin/env bash

IP="localhost"
PORT=3434

DATA='{"key1":"value1", "key2":"value2"}'

echo "Testing for port: ${PORT}"
printf "${DATA}" | curl -X POST "http://${IP}:${PORT}" \
	-H "Content-Type: application/json" \
	--trace-ascii output.log
