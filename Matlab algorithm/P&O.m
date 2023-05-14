function D = MPPT (Upv, Ipv, step)

persistent Prev Urev Drev

if isempty (Prev)
    Drev = 0.1;
    Urev = 0;
    Prev = 0;
end

P = Upv * Ipv;
dP = P - Prev;
dU = Upv - Urev;

if dP ~= 0
    if dP > 0
        if dU > 0
            D = Drev - step;
        else
            D = Drev + step;
        end
    else
        if dU > 0
            D = Drev + step;
        else
            D = Drev - step;
        end
    end
else
    D = Drev;
end


Drev = D;
Prev = P;
Urev = Upv;
end