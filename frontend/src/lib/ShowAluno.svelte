<script>

import { getData } from './service'

let data = []

getData("/usuarios").then((response) => {
    data = response
})

</script>

<div>
    <h2>Usuários</h2>
    {#each data as user}
        <p>{user.cpf} - {user.nome}</p>        
    {/each}
</div>

<style>
</style>