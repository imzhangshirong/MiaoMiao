package config

import(
	"time"
)

type SessionConfig struct{
	SessionPath string
	MaxLifeTime int64 //ms
	Salt string
	RandomSeed int64
	AutoCleanDuration int64
}

func (p *SessionConfig) Init()(*SessionConfig){
	p.SessionPath = "session"
	p.MaxLifeTime = 5 * 60 * 1000
	p.Salt = "miaomiao"
	p.RandomSeed = time.Now().Unix()
	p.AutoCleanDuration = 5 * 1000
	return p
}