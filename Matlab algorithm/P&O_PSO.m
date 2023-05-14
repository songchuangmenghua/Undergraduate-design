function D= MPPT(Upv, Ipv)

persistent Prev1 Prev2 Urev1 Urev2 Drev Drev1 Drev2 step1 step2 Dbest Dbest1 Dbest2 Pbest Pbest1 Pbest2 n

if isempty(Prev1)
    Prev1 = 0;
    Prev2 = 0;
    Urev1 = 0;
    Urev2 = 0;
    Drev = 0;
    Drev1 = 0;
    Drev2 = 0;
    step1 = 0.2;
    step2 = 0.5;
    Dbest = 0;
    Dbest1 = 0;
    Dbest2 = 0;
    Pbest = 0;
    Pbest1 = 0;
    Pbest2 = 0;
    n = 1;
    
    
end

P = Upv * Ipv;

if P == 0
    Drev = 0.2;
    Drev1 = Drev;
end

if P ~= 0
if n == 1
    
    dP = P - Prev1;
    dU = Upv - Urev1;
    step = 0.8 * step1 + 0.1 * 2 * (Dbest1 - Drev1) + 0.1 * 2 * (Dbest - Drev1);
    
    if step > 1
        step = 1;
    end
    
    if step < 0
        step = 0;
    end
    
    step1 = step;
    
    if P > Pbest1
        Dbest1 = Drev1;
        Pbest1 = P;
    end
    
    if dP > 0
        if dU > 0 
            Drev = Drev1 + step1;        
        else
            Drev = Drev1 - step1;
        end
    else
        if dU > 0 
            Drev = Drev1 - step1;
        else
            Drev = Drev1 + step1;
        end
    end
    
    if Drev > 1
        Drev = 1;
    end
    
    if Drev < 0
        Drev = 0;
    end
    
    Drev1 = Drev;
    Urev1 = Upv;
    Prev1 = P;
end

if n == 2
    
    dP = P - Prev2;
    dU = Upv - Urev2;
    step = 0.8 * step2 + 0.1 * 2 * (Dbest2 - Drev2) + 0.1 * 2 * (Dbest - Drev2);
    step2 = step;
    
    if step > 1
        step = 1;
    end
    
    if step < 0
        step = 0;
    end
    
    if P > Pbest2
        Dbest2 = Drev2;
        Pbest2 = P;
    end
    
    if dP > 0
        if dU > 0 
            Drev = Drev2 + step2;
        else
            Drev = Drev2 - step2;
        end
    else
        if dU > 0 
            Drev = Drev2 - step2;
        else
            Drev = Drev2 + step2;
        end
    end
    
    if Drev > 1
        Drev = 1;
    end
    
    if Drev < 0
        Drev = 0;
    end
    
	Drev2 = Drev;
    Urev2 = Upv;
    Prev2 = P;
end
end

n = n + 1;

if n == 3
    n = 1;
end

if n == 1 
    Drev = Drev2;
    Drev1 = Drev;
    Urev1 = Upv;
    Prev1 = P;
end

if n == 2
    Drev = Drev1;
    Drev2 = Drev;
    Urev2 = Upv;
    Prev2 = P;
end

if Pbest < Pbest1
    Pbest = Pbest1;
    Dbest = Dbest1;
end

if Pbest < Pbest2
    Pbest = Pbest2;
    Dbest = Dbest2;
end

D = Drev;

end