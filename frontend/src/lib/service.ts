const api_url = "http://localhost:8080"

export async function postData(path: String, data: Object) {
    const response = await fetch(`${api_url}${path}`, {
        method: "POST",
        headers: {
            "Content-Type": "application/json",
        },
        mode: "cors",
        body: JSON.stringify(data)
    });
    const response_data = await response.json()
    return response_data
}

export async function getData(path: String) {
    const response = await fetch(`${api_url}${path}`, {
        method: "GET",
        mode: "cors",
    });
    const response_data = await response.json()
    return response_data
}