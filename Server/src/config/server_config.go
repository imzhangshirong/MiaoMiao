package config

type ServerConfig struct{
	Host string
	Port int
}

func (p *ServerConfig) Init()(*ServerConfig){
	p.Host = "localhost"
	p.Port = 8000
	return p
}