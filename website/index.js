const express = require('express')
const path = require('path')

const app = express()
const port = 3000

app.get('/', (req, res) => res.sendFile(path.join(__dirname + '/index.htm')))
app.get('/about', (req, res) => res.sendFile(path.join(__dirname + '/about.htm')))
app.get('/data', (req, res) => res.sendFile(path.join(__dirname + '/data.htm')))

app.listen(port, () => console.log(`Test server at http://localhost:${port}`))