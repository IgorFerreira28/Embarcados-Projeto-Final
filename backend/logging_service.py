from flask import Blueprint, request, jsonify
import sqlite3
from datetime import datetime

logging_bp = Blueprint('logging_service', __name__)

# Inicializar banco de dados
def init_db():
    conn = sqlite3.connect("database.db")
    cursor = conn.cursor()
    cursor.execute("""
        CREATE TABLE IF NOT EXISTS logs (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            valor_sensor INTEGER,
            timestamp TEXT
        )
    """)
    conn.commit()
    conn.close()

init_db()

@logging_bp.route("/post_logs", methods=["POST"])
def log_sensor():
    try:
        # Captura os dados do JSON enviado pelo ESP8266
        dados = request.json
        valor_sensor = dados.get("valor_sensor")
        print(f"Valor do sensor recebido: {valor_sensor}")

        # Conectar ao banco de dados SQLite
        conn = sqlite3.connect("database.db")
        cursor = conn.cursor()

        # Inserir os dados no banco de dados
        cursor.execute("INSERT INTO logs (valor_sensor, timestamp) VALUES (?, ?)",
                       (valor_sensor, datetime.now()))
        conn.commit()
        conn.close()

        # Retornar resposta ao ESP8266
        return jsonify({"mensagem": "Log registrado!"}), 200

    except Exception as e:
        print(f"Erro ao registrar log: {e}")
        return jsonify({"mensagem": "Erro ao registrar log!"}), 500

@logging_bp.route("/get_logs", methods=["GET"])
def get_logs():
    conn = sqlite3.connect("database.db")
    cursor = conn.cursor()
    cursor.execute("SELECT * FROM logs")
    logs = cursor.fetchall()
    conn.close()

    return jsonify(logs)
