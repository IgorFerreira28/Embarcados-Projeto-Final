from flask import Blueprint, request, jsonify

controle_bp = Blueprint('controle_service', __name__)

# Threshold inicial
configuracao = {"threshold": 512}

@controle_bp.route("/get_config", methods=["GET"])
def get_configuracao():
    return jsonify(configuracao["threshold"])

@controle_bp.route("/set_config", methods=["POST"])
def set_configuracao():
    dados = request.json
    configuracao["threshold"] = dados.get("threshold", 512)
    return jsonify({"mensagem": "Configuração atualizada!"})
