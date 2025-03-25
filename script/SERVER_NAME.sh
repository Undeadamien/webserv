#!/bin/env bash

IP="localhost"
PORT=3434

echo "Testing for port: ${PORT}"
curl -X POST "http://${IP}:${PORT}" \
	--trace-ascii output.log
