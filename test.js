var sqlite = require('.')

console.log(sqlite)

var db = Buffer.alloc(sqlite.PTR_WIDTH)

console.log(db)

console.log(sqlite.sqlite3_open(':memory:', db))

var stmt = Buffer.alloc(sqlite.PTR_WIDTH)

console.log(sqlite.sqlite3_prepare(db, "SELECT SQLITE_VERSION()", stmt))

console.log(db)
console.log(stmt)
console.log(sqlite.sqlite3_errmsg(db))

console.log(sqlite.sqlite3_close(db))

console.log(sqlite.sqlite3_errmsg(db))

console.log(db)
