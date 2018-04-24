package config

type MySqlConfig struct{
	Database string
	Host string
	Port int
	User string
	Password string
	Charset string
}

func (p *MySqlConfig) Init()(*MySqlConfig){
	p.Database = "miaomiao"
	p.Host = "localhost"
	p.Port = 3306
	p.User = "root"
	p.Password = "root"
	p.Charset = "utf8"
	return p
}