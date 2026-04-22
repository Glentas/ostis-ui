import { ScClient } from "ts-sc-client";

const client = new ScClient('https://localhost:8090');

client.addEventListener("open", () => {
    console.log("Подключено к sc-серверу");
});

client.addEventListener("error", (error) => {
    console.error("Ошибка подключения:", error);
});