CREATE TABLE usuario (
    cpf             VARCHAR(11) PRIMARY KEY NOT NULL,
    nome            VARCHAR(50) NOT NULL,
    matricula       INT,
    data_nascimento DATE
);

CREATE TABLE usuario_foto (
    usuario VARCHAR(11) NOT NULL,
    foto BYTEA NOT NULL,

    CONSTRAINT FK_usuario_foto_usuario FOREIGN KEY(usuario) REFERENCES usuario(cpf)
);

CREATE TYPE prova_status AS ENUM ('marcada', 'finalizada');

CREATE TABLE prova (
    id              UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    local_aplicacao VARCHAR(225) NOT NULL,
    horario_inicio  TIMESTAMP NOT NULL,
    horario_fim     TIMESTAMP NOT NULL,
    status          prova_status DEFAULT 'marcada'
);

CREATE TABLE usuario_prova (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    usuario VARCHAR(11) NOT NULL,
    prova UUID NOT NULL,
    autenticacao BOOLEAN NOT NULL DEFAULT false,

    CONSTRAINT FK_usuario_prova_usuario FOREIGN KEY(usuario) REFERENCES usuario(cpf),
    CONSTRAINT FK_usuario_prova_prova FOREIGN KEY(prova) REFERENCES prova(id)
);