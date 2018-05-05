package app

import(
	config "config"
	router "package/router"
	dbutil "package/dbutil"
	session "package/session"
)

var Router *router.Router
var DBUtil *dbutil.DBUtil
var SessionManager *session.SessionManager

func init(){
	Router = new(router.Router).Init(
		config.Server.Host,
		config.Server.Port)
	DBUtil = new(dbutil.DBUtil).Init(
		config.MySql.Host,
		config.MySql.Port,
		config.MySql.User,
		config.MySql.Password,
		config.MySql.Database,
		config.MySql.Charset)
	SessionManager = new(session.SessionManager).Init(
		config.Session.SessionPath,
		config.Session.Salt,
		config.Session.MaxLifeTime,
		config.Session.RandomSeed,
		config.Session.AutoCleanDuration,
	)
}