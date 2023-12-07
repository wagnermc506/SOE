import enum
from database import Base
from sqlalchemy import Column, Integer, String, TIMESTAMP, Boolean, Text, UUID, DATETIME, ForeignKey, Enum, DATE, BLOB

class Usuario(Base):
    __tablename__ = "usuario"

    cpf = Column(String(11), primary_key=True, nullable=False)
    nome = Column(String(40), nullable=False)
    matricula = Column(Integer, nullable=True)
    data_nascimento = Column(DATE, nullable=True)
    # caminho_foto = Column(String(100), nullable=True)

class UsuarioFoto(Base):
    __tablename__ = "usuario_foto"

    usuario = Column(ForeignKey("usuario.cpf"), primary_key=True, nullable=False)
    foto = Column(BLOB, nullable=False)

class Prova(Base):
    __tablename__ = "prova"

    id = Column(UUID, primary_key=True, nullable=False)
    local_aplicacao = Column(String, nullable=False)
    horario_inicio = Column(TIMESTAMP, nullable=False)
    horario_fim = Column(TIMESTAMP, nullable=False)
    status = Column(String(12), default='marcada')

class UsuarioProva(Base):
    __tablename__ = "usuario_prova"

    id = Column(UUID, primary_key=True)
    usuario = Column(ForeignKey("usuario.cpf"), nullable=False)
    prova = Column(ForeignKey("prova.id"), nullable=False)
    autenticacao = Column(Boolean, nullable=False, default=False)
    # presente = Column(Boolean, nullable=False, default=False)
