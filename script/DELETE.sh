#!/bin/env bash

IP="localhost"
PORT=3434

echo "Testing for port: ${PORT}"
curl "http://localhost:${PORT}" \
	-H "Content-Type: application/json" \
	-d '{"key1":"value1", "key2":"value2"}' \
	-X POST
