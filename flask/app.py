from flask import Flask, jsonify
import json
import os

app = Flask(__name__)
METRICS_FILE = '/tmp/metrics.json'

@app.route('/metrics', methods=['GET'])
def get_metrics():
    if os.path.exists(METRICS_FILE):
        try:
            with open(METRICS_FILE, 'r') as f:
                data = json.load(f)
            return jsonify(data)
        except Exception as e:
            return jsonify({'error': 'Error reading metrics', 'details': str(e)}), 500
    else:
        return jsonify({'error': 'Metrics file not found'}), 404

if __name__ == '__main__':
    # Run the app on all interfaces at port 5000
    app.run(host='0.0.0.0', port=5000, debug=True)
