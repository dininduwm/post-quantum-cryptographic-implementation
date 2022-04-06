/*
    recover K from c0,c1
    c1-sT*c0 = K.q/2 - eTR
    // we have K now
    plain = DECRYPTaes_ctr(C1,K)

    vT = c3T - c2T.D
    h <---- v
        if v[i] ~ 0 then h[i] = 0
        else h[i] ~ 1.
        QUESTION : WHAT IS THE RANGE OF V? [-q/2,q/2] ?

*/