#!/bin/bash
# start_services.sh: Start the system_monitor daemon and the Flask API

# Start the system_monitor daemon (assumes it was built in src/)
echo "Starting system_monitor daemon..."
./src/system_monitor &

# Give the daemon a couple of seconds to start generating metrics
sleep 2

# Start the Flask API
echo "Starting Flask API..."
cd flask
export FLASK_APP=app.py
flask run --host=0.0.0.0 --port=5000
