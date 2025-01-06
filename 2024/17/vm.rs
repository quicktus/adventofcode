
pub struct State {
    reg_a: u128,
    reg_b: u128,
    reg_c: u128,
    ip: usize,
    pub stdout: Vec<u8>,
}

impl State {
    pub fn new(regs: Vec<u128>) -> Self {
        let (reg_a, reg_b, reg_c) = (regs[0], regs[1], regs[2]);
        State { reg_a, reg_b, reg_c, ip: 0, stdout: Vec::new() }
    }
}

pub fn interpret(state: &mut State, program: &Vec<u8>) {
    loop {
        if state.ip >= program.len()-1 {
            break;  // halt
        }
        let opcode = program[state.ip];
        let operand = program[state.ip+1];
        state.ip += 2;

        match opcode {
            0 => xdv(state, operand, 0),
            1 => bxl(state, operand),
            2 => bst(state, operand),
            3 => jnz(state, operand),
            4 => bxc(state, operand),
            5 => out(state, operand),
            6 => xdv(state, operand, 1),
            7 => xdv(state, operand, 2),
            _ => panic!("unknown opcode!")
        }
    }
}

/// The xdv function performs division.
/// It combines the instructions adv, bdv, and cdv (opcodes 0, 6, 7).
/// The numerator is the value in the A register.
/// The denominator is found by raising 2 to the power of the instruction's combo operand.
/// The result of the division operation is truncated to an integer
/// The result is stored in register {adv->A; bdv->B; cdv->C}.
fn xdv(state: &mut State, operand: u8, reg_nr: u8) {
    let exponent = combo(state, operand);
    let res = state.reg_a >> exponent;
    let dest = match reg_nr {
        0 => &mut (state.reg_a),
        1 => &mut (state.reg_b),
        2 => &mut (state.reg_c),
        _ => panic!("unknown operation!")
    };
    *dest = res;
}

/// The bxl instruction (opcode 1) calculates the bitwise XOR of register B and the instruction's literal operand.
/// The result is stored in register B.
fn bxl(state: &mut State, operand: u8) {
    state.reg_b ^= operand as u128;
}

/// The bst instruction (opcode 2) calculates the value of its combo operand modulo 8 (thereby keeping only its lowest 3 bits).
/// The result is stored in register B.
fn bst(state: &mut State, operand: u8) {
    state.reg_b = combo(state, operand) & 0b111;
}

/// The jnz instruction (opcode 3) does nothing if the A register is 0.
/// If the A register is not zero, it jumps by setting the instruction pointer to the value of its literal operand
/// If this instruction jumps, the instruction pointer is not increased by 2 after this instruction.
fn jnz(state: &mut State, operand: u8) {
    if state.reg_a == 0 {
        return;
    }
    else {
        state.ip = operand as usize;
    }
}

/// The bxc instruction (opcode 4) calculates the bitwise XOR of register B and register C.
/// For legacy reasons, this instruction comsumes an operand but ignores it.
/// The result is stored in register B.
fn bxc(state: &mut State, _operand: u8) {
    state.reg_b ^= state.reg_c;
}

/// The out instruction (opcode 5) calculates the value of its combo operand modulo 8
/// The result is printed to the outputs stream.
/// If a program outputs multiple values, they are separated by commas.
fn out(state: &mut State, operand: u8) {
    let out_val = (combo(state, operand) & 0b111).try_into().unwrap();
    state.stdout.push(out_val);
}

fn combo(state: &mut State, operand: u8) -> u128 {
    match operand {
        lit if lit <= 3 => lit as u128,  // Combo operands 0 through 3 represent literal values 0 through 3.
        4 => state.reg_a,  // Combo operand 4 represents the value of register A.
        5 => state.reg_b,  // Combo operand 5 represents the value of register B.
        6 => state.reg_c,  // Combo operand 6 represents the value of register C.
        _ => panic!("undefined combo operand!")  // Combo operand 7 is reserved and will not appear in valid programs.
    }
}
