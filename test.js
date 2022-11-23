const http = require("http");
const url = require('url')
const fs = require("fs");
var addon = require('bindings')('test');


http.createServer(function(request,response){
    if (request.url.search("result")!=-1)
    {
        response.writeHead(200, {'Content-Type': 'text/html; charset=utf-8'});
        const queryObject = url.parse(request.url, true).query;
        username = queryObject['username']
        if (addon.check_user(username))
        {
            response.write("Пользователь есть");
        }
        else
        {
            response.write("Пользователя нет");
        }
        response.write("<form action=\"http://127.0.0.1:3000\">")
        response.write("<input type=\"submit\" value=\"Вернуться назад\" />")
        response.write("</form>")
        response.end()
    }
    fs.readFile("index.html", function(error, data) 
    {
        response.end(data)
    });
     
}).listen(3000, "127.0.0.1",function(){
    console.log("Сервер начал прослушивание запросов на порту 3000");
    addon.launch_browser("http://127.0.0.1:3000");
});