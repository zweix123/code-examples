// copy from sync.once and modify
package multiple

import (
	"sync"
	"sync/atomic"
)

type Multiple struct {
	done atomic.Uint32
	m    sync.Mutex
}

func (mt *Multiple) Do(f func()) {
	if mt.done.Load() == 0 {
		mt.doSlow(f)
	}
}

// 重置状态, 使得下次调用Do时, 会再次执行f
// 该函数是线程安全的
// 该函数是幂等的, 即多次调用效果和一次调用一样, 不会对调用次数进行计数而导致可以Do多次, 多次调用也是只能Do一次; 只有Do过的Renew才能重新Do
func (mt *Multiple) Renew() {
	mt.m.Lock()
	defer mt.m.Unlock()
	mt.done.Store(0)
}

// 该函数是有必须要需要单独的, 因为defer是函数纬度的不是块纬度的
func (mt *Multiple) doSlow(f func()) {
	mt.m.Lock()
	defer mt.m.Unlock()
	if mt.done.Load() == 0 {
		defer mt.done.Store(1)
		f()
	}
}
