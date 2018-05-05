package session

import(
	"math/rand"
	"net/http"
    "strconv"
    "sync"
	"time"
	"encoding/gob"
	"io/ioutil"
	"os"
	"bytes"

	debug "package/debug"
	common "package/common"
)

type Session struct{
	Sid string
	LastTime time.Time
	Data map[string]interface{}
	Modified bool
}

func (p *Session) Init(){
	p.Data = make(map[string]interface{})
	p.Modified = true
}

func (p *Session) Set(key string,data interface{}){
	p.Data[key] = data
	p.Modified = true
}

func (p *Session) Get(key string)interface{}{
	if v,ok:=p.Data[key];ok{
		return v
	}
	return nil
}

type SessionManager struct{
	salt string
	maxLifeTime int64
	lock sync.Mutex
	rwLock sync.RWMutex
	sessionData map[string]Session
	random *rand.Rand
	sessionPath string
	autoCleanDuration int64
	ticker *time.Ticker
}

func init(){

}

func (p *SessionManager) Init(sessionPath string,salt string,maxLifeTime int64,seed int64,autoCleanDuration int64)(*SessionManager){
	p.sessionPath = sessionPath
	p.salt = salt
	p.maxLifeTime =maxLifeTime
	p.random = rand.New(rand.NewSource(seed))
	p.sessionData = make(map[string]Session)
	p.autoCleanDuration = autoCleanDuration
	return p
}

func (p *SessionManager) AutoClean(){
	p.ticker = time.NewTicker(time.Duration(p.autoCleanDuration)*time.Millisecond)
    go func() {
        for _ = range p.ticker.C {
            p.Clean()
        }
    }()
}

func (p *SessionManager) StopAutoClean(){
	p.ticker.Stop()
}

func (p *SessionManager) Create() Session {
	timeN := time.Now()
	b := p.salt+strconv.FormatInt(timeN.Unix(),10)+strconv.FormatInt(p.random.Int63(),10)
	sid := common.MD5(b)
	session := Session{Sid:sid,LastTime:timeN}
	session.Init()
	p.sessionData[session.Sid] = session
	return session
}

func (p *SessionManager) Get(sid string) (Session,bool){
	p.lock.Lock()
	defer p.lock.Unlock()
	if v,ok := p.sessionData[sid];ok {
		v.LastTime = time.Now()
		return v,true
	}
	return Session{},false
}

func (p *SessionManager) Destroy(sid string) bool{
	p.lock.Lock()
	defer p.lock.Unlock()
	if _,ok := p.sessionData[sid];ok {
		delete(p.sessionData,sid)
		os.Remove(p.sessionPath+string(os.PathSeparator)+sid)
		return true
	}
	return false
}

func (p *SessionManager) Clean(){
	var outTime []string
	tNow:= time.Now()
	for k,v := range p.sessionData{
		if int64(tNow.Sub(v.LastTime).Seconds()*1000) > p.maxLifeTime {
			outTime = append(outTime,k)
		}
	}
	for _,v := range outTime{
		debug.Log("Clean:"+v)
		p.Destroy(v)
	}
}

func (p *SessionManager) GetFromClient(writer http.ResponseWriter,request *http.Request) (Session){
	cookie,err:=request.Cookie("session")
	var session Session
	ok:=true
	if err!=nil {
		ok = false
	}else{
		sid:=cookie.Value
		session,ok=p.Get(sid)
	}
	if !ok {
		session = p.Create()
		cookie := http.Cookie{
			Name:"session",
			Value:session.Sid,
			MaxAge:int(p.maxLifeTime/1000),
		}
		http.SetCookie(writer,&cookie)
	}
	return session
}

func (p *SessionManager) Save(session Session){
	path:=p.sessionPath+string(os.PathSeparator)+session.Sid
	p.rwLock.Lock()
	defer p.rwLock.Unlock()
	var fout bytes.Buffer        
    enc := gob.NewEncoder(&fout)
    err := enc.Encode(session)
    if err != nil {
		debug.Error(err.Error())
		return
    }
	ioutil.WriteFile(path, fout.Bytes(), 0644)
	session.Modified = false
}

func (p *SessionManager) LoadSession(sid string) (Session,bool){
	file:=p.sessionPath+string(os.PathSeparator)+sid
	p.rwLock.RLock()
	defer p.rwLock.RUnlock()
	fin,err := os.Open(file)
	if err !=nil{
		debug.Error(err.Error())
		return Session{},false
	}
	dec := gob.NewDecoder(fin)
    var session Session
	err = dec.Decode(&session)
	if err !=nil{
		debug.Error(err.Error())
		return Session{},false
	}
	return session,true
}

func (p *SessionManager) ImportSession(){
	files,err := common.ListDir(p.sessionPath,"")
	if err!=nil {
		debug.Error(err.Error())
	}
	for _,v := range files{
		session,_:=p.LoadSession(v)
		if session.Sid!=""{
			p.sessionData[session.Sid] = session			
		}
	}
}
