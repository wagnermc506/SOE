from fastapi import FastAPI, Depends, UploadFile, Response
from pydantic import BaseModel
from sqlalchemy.orm import Session
import models
from database import engine, get_db
from uuid import UUID, uuid4
from datetime import datetime, date
from typing import Union, Optional, List
from fastapi.middleware.cors import CORSMiddleware

models.Base.metadata.create_all(bind=engine)

class User(BaseModel):
    cpf: Union[str, None] = None
    nome: str
    matricula: int
    data_nascimento: date

    class Config:
        orm_mode = True

class UserList(BaseModel):
    cpfs: List[str] = []

class Prova(BaseModel):
    id: Optional[UUID] = None
    local_aplicacao: str
    horario_inicio: datetime
    horario_fim: datetime

    class Config:
        orm_mode = True

class UserPresent(BaseModel):
    cpf: str
    presenca: bool
    auth: bool = False

origins = [
    "http://localhost:8080",
    "http://localhost:5173"
]

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

@app.post("/usuario")
def create_user(user: User, db: Session = Depends(get_db)):
    new_user = models.Usuario(**user.dict())
    db.add(new_user)
    db.commit()
    db.refresh(new_user)

    return [new_user]

@app.post("/usuario/{cpf}/foto")
def add_photo(cpf: str, file: UploadFile, db: Session = Depends(get_db)):
    user = db.query(models.Usuario).filter_by(cpf = cpf).first()
    user_photo = models.UsuarioFoto()
    user_photo.usuario = user.cpf
    user_photo.foto = file.file.read()
    db.add(user_photo)
    db.commit()

    return {"filename": file.filename}

@app.get("/usuario/{cpf}/foto")
def get_photo(cpf: str, db: Session = Depends(get_db)):
    user_photo = db.query(models.UsuarioFoto).filter_by(usuario = cpf).first()
    return Response(content=user_photo.foto, media_type="image/jpeg")

@app.get("/usuario/{cpf}")
def get_user(cpf: str, db: Session = Depends(get_db)):
    user = db.query(models.Usuario).filter_by(cpf = cpf).first()
    return user

@app.get("/usuarios")
def get_users(db: Session = Depends(get_db)):
    users = db.query(models.Usuario).all()
    return users

@app.delete("/usuario/{cpf}")
def delete_user(cpf: str, db: Session = Depends(get_db)):
    user = db.query(models.Usuario).filter_by(cpf = cpf)
    user.delete()
    db.commit()

@app.put("/usuario/{cpf}")
def update_user(cpf: str, user: User, db: Session = Depends(get_db)):
    up_user = db.query(models.Usuario).filter_by(cpf = cpf)
    up_user.update(user.dict())
    db.commit()
    return up_user.first()

@app.post("/prova")
def create_exam(prova: Prova, db: Session = Depends(get_db)):
    # prova.status = models.StatusProva["marcada"]
    
    new_exam = models.Prova(**prova.dict())
    new_exam.id = str(uuid4())
    db.add(new_exam)
    db.commit()
    db.refresh(new_exam)
    return new_exam

@app.get("/provas")
def list_exams(db: Session = Depends(get_db)):
    return db.query(models.Prova).all()

@app.get("/prova/{uuid}")
def get_exam(uuid: str, db: Session = Depends(get_db)):
    exam = db.query(models.Prova).filter_by(id = uuid).first()
    return exam

@app.delete("/prova/{uuid}")
def delete_exam(uuid: str, db: Session = Depends(get_db)):
    exam = db.query(models.Prova).filter_by(id = uuid)
    exam.delete()
    db.commit()

@app.put("/prova/{uuid}")
def update_exam(uuid: str, prova: Prova, db: Session = Depends(get_db)):
    exam = db.query(models.Prova).filter_by(id = uuid)
    p = prova.dict()
    p.pop('id')
    exam.update(p)
    db.commit()
    return exam.first()

@app.post("/prova/{uuid}/registrar_usuarios")
def register_user_to_exam(uuid: str, users: UserList, db: Session = Depends(get_db)):
    for cpf in users.cpfs:
        usuario_prova = models.UsuarioProva()
        usuario_prova.id = str(uuid4())
        usuario_prova.prova = uuid
        usuario_prova.usuario = cpf
        db.add(usuario_prova)
    db.commit()

@app.put("/prova/{uuid}/confirmarpresenca")
def update_user_presence(uuid: str, user: UserPresent, db: Session = Depends(get_db)):
    usuario_prova = db.query(models.UsuarioProva).filter_by(prova = uuid, usuario = user.cpf)
    usuario_prova.update({"autenticacao": user.auth, "presente": user.presenca})
    db.commit()

@app.get("/prova/{uuid}/usuarios")
def get_users_from_exam(uuid: str, db: Session = Depends(get_db)):
    usuarios_prova = db.query(models.UsuarioProva).filter_by(prova = uuid).all()
    users = list(map(lambda x: x.usuario, usuarios_prova))
    return db.query(models.Usuario).filter(models.Usuario.cpf.in_(users)).all()

@app.delete("/prova/{uuid}/usuario/{cpf}")
def remove_user_from_exam(uuid: str, cpf: str, db: Session = Depends(get_db)):
    usuarios_prova = db.query(models.UsuarioProva).filter_by(prova = uuid, usuario = cpf)
    usuarios_prova.delete()
    db.commit()
