<script>

import { postData } from './service'

let cpf = "07340744178";
let nome = "Wagner";
let matricula = "180029177";
let data_nascimento = "2000-02-09";
let response = {};

async function sendData() {
    const response = await postData("/usuario", {
        cpf: cpf,
        nome: nome,
        matricula: matricula,
        data_nascimento: data_nascimento
    })
    console.log(response)
}

</script>

<div>
    <h2>Cadastrar Usuário</h2>
    <label>CPF: <input bind:value={cpf}/></label><br/>
    <label>Nome: <input bind:value={nome}/></label><br/>
    <label>Matricula: <input bind:value={matricula}/></label><br/>
    <label>Data de Nascimento: <input bind:value={data_nascimento}/></label><br/>
    <button on:click={sendData}>Criar Usuário</button>
</div>