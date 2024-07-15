function getProjectBinariesDir(projectName)
    return "../Binaries/" .. projectName .. "/%{cfg.buildcfg}-" .. firstToUpper(os.target()) .. "-%{cfg.architecture}"
end

function getProjectIntermediateDir(projectName)
    return "../Intermediate/" .. projectName .. "/%{cfg.buildcfg}-" .. firstToUpper(os.target()) .. "-%{cfg.architecture}"
end
