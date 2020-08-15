// Test of the mem interface
// (c) Maddie Burbage, August 2020, for the Bailey Research Group at Williams

package accelerators

import Chisel._
import freechips.rocketchip.tile._ //For LazyRoCC
import freechips.rocketchip.config._ //For Config
import freechips.rocketchip.diplomacy._ //For LazyModule
import freechips.rocketchip.rocket.{HellaCacheReq} //For cache accesses

//Wrapper for the accelerator
class MemTest(opcodes: OpcodeSet)(implicit p: Parameters) extends LazyRoCC(opcodes) {
    override lazy val module = new MemTestImp(this)
}

// Main accelerator class
class MemTestImp(outer: MemTest)(implicit p: Parameters) extends LazyRoCCModuleImp(outer) {
  // Basic state
  val s_idle :: s_busy :: s_wait :: s_resp :: Nil = Enum(Bits(), 4)
  val state = Reg(init = s_idle) //Idle until handling an instruction
  io.interrupt := Bool(false)

  // Instruction inputs
  val address = Reg(init = 0.U(64.W))
  val data = Reg(init = 0.U(64.W))
  val typ = Reg(init = 0.U(2.W)) //Can be 3 as well but we don't have room in inst
  val command = Reg(init = 0.U(5.W))
  val rd = Reg(init = 0.U(5.W))
  val returned = Reg(init = 0.U(64.W))

  //RoCC interface
  io.cmd.ready := (state === s_idle)
  io.resp.valid := (state === s_resp)

  //Memory request interface
  io.mem.req.valid := (state === s_busy)
  io.busy := (state === s_busy) || (state === s_wait)
  io.mem.req.bits.addr := address
  io.mem.req.bits.tag :=  address(5,0)
  io.mem.req.bits.cmd := command
  io.mem.req.bits.data := data
  io.mem.req.bits.size := typ
  io.mem.req.bits.signed := Bool(false)
  //io.mem.req.bits.typ := typ
  io.mem.req.bits.phys := Bool(false)

  //Response interface
  io.resp.bits.data := returned
  io.resp.bits.rd := rd

  when(io.cmd.fire()) {
    state := s_busy

    address := io.cmd.bits.rs1
    data := io.cmd.bits.rs2
    typ := io.cmd.bits.inst.funct(6,5)
    command := io.cmd.bits.inst.funct(4,0)
    rd := io.cmd.bits.inst.rd
    printf("Got instruction for cmd: %x at addr: %x with typ: %x and data: %x\n", io.cmd.bits.inst.funct(4,0), io.cmd.bits.rs1, io.cmd.bits.inst.funct(6,5), io.cmd.bits.rs2)
  }

  when(io.mem.req.fire()) {
    state := s_wait
    printf("Sent to mem cmd: %x at addr: %x with data %x at size %x and tag %x \n", io.mem.req.bits.cmd, address, io.mem.req.bits.data, typ, address(5,0))
  }

  when(io.mem.resp.valid) {
    state := s_resp
    assert(io.mem.resp.bits.addr === address)
    assert(io.mem.resp.bits.tag === address(5,0))
    printf("Received from mem cmd: %x at addr: %x with data %x at size %x and tag %x \n", io.mem.resp.bits.cmd, io.mem.resp.bits.addr, io.mem.resp.bits.data, io.mem.resp.bits.size, io.mem.resp.bits.tag)
    returned := io.mem.resp.bits.data
  }

  when(io.resp.fire()) {
    state := s_idle
  }

}

//Setup for the accelerator
class WithMemTest extends Config((site, here, up) => {
    case BuildRoCC => Seq((p: Parameters) => {
        val MemTest = LazyModule.apply(new MemTest(OpcodeSet.custom0) (p))
        MemTest
    })
})
