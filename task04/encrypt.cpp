/*
    A(nxm) <-[0,q-1]    | public
    b(mx1)  | public
    D(mxk) <-{0,1}  | public
    U(nxk) =AD  | public
    H hash function | public
*/

/*
    K key <- {0,1} | length will be 256
    C1 = ENCRYPTaes_ctr(plain,K)
    R(mx256) <- {0,1}
    c0 = AR
    c1T = bT.R+k*ceil(q/2)
    r(nx1) <- [0,q)
    x(mx1) <- N(alpha)
    y(kx1) <- N(alpha)
    c2T = rT.A+xT
    c3T = rT.U + yT + H(plain)* floor(q/2)

    C2 = [c0,c1,c2,c3]
*/


