from flask import Flask, jsonify, request
from flask_cors import CORS
from controle_service import controle_bp
from logging_service import logging_bp

app = Flask(__name__)
CORS(app)

# Registrar os blueprints (microserviços)
app.register_blueprint(controle_bp, url_prefix='/controle')
app.register_blueprint(logging_bp, url_prefix='/logging')

@app.route("/")
def home():
    return jsonify({"mensagem": "API Gateway funcionando!"})

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
