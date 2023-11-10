<script>

import { postData } from "./service";

let local_aplicacao = "cef 304"
let horario_inicio = "2023-12-01 13:00:00"
let horario_fim = "2023-12-01 14:00:00"

async function sendData() {
    const response = await postData("/prova", {
        local_aplicacao: local_aplicacao,
        horario_inicio: horario_inicio,
        horario_fim: horario_fim
    })
    console.log(response)
}

</script>

<div>
    <h2>Cadastrar Prova</h2>
    <label>Local de Aplicação: <input bind:value={local_aplicacao}/></label><br/>
    <label>Horário Inicio: <input bind:value={horario_inicio}/></label><br/>
    <label>Horário Fim: <input bind:value={horario_fim}/></label><br/>
    <button on:click={sendData}>Cadastrar Prova</button>
</div>